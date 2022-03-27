import argparse
import os
import numpy as np
from PIL import Image
import pandas as pd
from matplotlib import pyplot as plt


class CGLMDescriptors(object):

    def __init__(self, cglm):
        self.cglm = cglm

        self.asm = 0
        self.entropy = 0
        self.contrast = 0
        self.variance = 0
        self.homogeneity = 0

        for i in range(self.cglm.shape[0]):
            for j in range(self.cglm.shape[1]):
                self.asm += (self.cglm[i][j]) ** 2.
                self.entropy += self.cglm[i][j] * (-np.log(self.cglm[i][j]) if self.cglm[i][j] > 0 else 0)
                self.contrast += self.cglm[i][j] * (i - j) ** 2.
                self.variance += self.cglm[i, j] * abs(i - j)  # também conhecida por dissimilaridade
                self.homogeneity += (self.cglm[i][j]) / (1 + (i - j) ** 2.)


def main(args):
    files = os.listdir(args.path)

    class_names = ['dentina', 'canal', 'pinos', 'background']
    glcm = ['0', '1', '2']
    glcm_mask = ['(0, 3)', '(3, 0)', '(3, -3)']

    columns = ['asm', 'entropy', 'contrast', 'variance', 'homogeneity']

    dict_data = {c: {x: [] for x in glcm} for c in class_names}
    limits = {c: {'min': np.inf, 'max': -np.inf} for c in columns}

    print('n_files: %d' % len(files))

    train_data_raw = []

    for i, some_file in enumerate(files):
        header = some_file.split('_')
        some_class = header[-2]
        glcm_index = header[-1].split('.')[0]

        img = Image.open(os.path.join(args.path, some_file))  # type: Image
        img = img.convert("L")
        data = np.asarray(img, dtype=np.int64) / 255.
        descriptor = CGLMDescriptors(data)

        dict_data[some_class][glcm_index] += [descriptor]
        train_data_raw += [[glcm_mask[int(glcm_index)]] + [
            getattr(descriptor, d) for d in columns] + [some_class]
                           ]

        for col in columns:
            limits[col]['min'] = min(limits[col]['min'], eval('descriptor.%s' % col))
            limits[col]['max'] = max(limits[col]['max'], eval('descriptor.%s' % col))

        print(i, '/', len(files))

    train_data = pd.DataFrame(train_data_raw, columns=['mask'] + columns + ['class'])
    train_data.to_csv('glcm_training_set.csv', index=False)

    for m, dm in zip(glcm, glcm_mask):
        fig, axes = plt.subplots(len(class_names), len(columns), figsize=(16, 10))
        axes = axes.ravel()
        counter_axes = 0
        for c in class_names:
            axes[counter_axes].set_ylabel(c)
            for descriptor in columns:
                axes[counter_axes].hist(
                    eval('np.array([x.%s for x in dict_data[c][m]])' % descriptor),
                    bins=30,
                    range=(limits[descriptor]['min'], limits[descriptor]['max'])
                )
                axes[counter_axes].set_title('%s %s' % (descriptor, dm))
                counter_axes += 1

        plt.subplots_adjust(hspace=0.3)

        plt.savefig('gclm_%s.pdf' % dm, format='pdf')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Script for batch-resizing images.'
    )

    parser.add_argument(
        '--path', action='store', required=True,
        help='Path where files to convert are.'
    )

    some_args = parser.parse_args()

    main(some_args)
