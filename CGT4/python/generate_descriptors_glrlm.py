import argparse
import os
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt


class GLRLMDescriptors(object):

    def __init__(self, cglm):

        self.cglm = cglm

        self.n_runs = len(self.cglm.columns)
        self.n_pixels = 640 * 640  # imagens com essas dimensões, em tons de cinza

        self.rp = self.n_runs / self.n_pixels
        self.sre = 0
        self.lre = 0
        self.gln = 0
        self.rln = 0
        self.lgre = 0
        self.hgre = 0
        self.srlge = 0
        self.srhge = 0
        self.lrlge = 0
        self.lrhge = 0

        M = len(self.cglm.columns)
        N = len(self.cglm.index)

        _rln_partial = 0.
        for ir in range(N):
            for k in range(1, M + 1):
                rik = self.cglm.loc[(slice(None), slice(None), ir), str(k)].values[0]
                _rln_partial += rik
            self.rln += _rln_partial ** 2.

        for k in range(1, M + 1):
            _gln_partial = 0.

            for ir in range(N):
                rik = self.cglm.loc[(slice(None), slice(None), ir), str(k)].values[0]

                i = ir + 1

                self.sre += rik / (k ** 2.)
                self.lre += rik * (k ** 2.)

                _gln_partial += rik

                self.lgre += rik / (i ** 2.)
                self.hgre += rik * (i ** 2.)
                self.srlge += rik / ((i ** 2.) * (k ** 2.))
                self.srhge += (rik * (i ** 2.)) / k ** 2.
                self.lrlge += (rik * (k ** 2.)) / (i ** 2.)
                self.lrhge += rik * (i ** 2.) * (k ** 2.)

            self.gln += _gln_partial ** 2.

        self.sre *= (1./float(self.n_runs))
        self.lre *= (1./float(self.n_runs))
        self.gln *= (1./float(self.n_runs))
        self.rln *= (1./float(self.n_runs))
        self.lgre *= (1./float(self.n_runs))
        self.hgre *= (1./float(self.n_runs))
        self.srlge *= (1./float(self.n_runs))
        self.srhge *= (1./float(self.n_runs))
        self.lrlge *= (1./float(self.n_runs))
        self.lrhge *= (1./float(self.n_runs))


def main(args):
    files = os.listdir(args.path)

    class_names = ['dentina', 'canal', 'pinos', 'background']
    glrlm = ['0', '1', '2', '3']
    glrlm_mask = ['(0, 1)', '(1, 0)', '(1, -1)', '(-1, 1)']

    columns = ['sre', 'lre', 'gln', 'rln', 'lgre', 'hgre', 'srlge', 'srhge', 'lrlge', 'lrhge']

    dict_data = {c: {x: [] for x in glrlm} for c in class_names}
    limits = {c: {'min': np.inf, 'max': -np.inf} for c in columns}

    print('n_files: %d' % len(files))

    train_data_raw = []

    for i, some_file in enumerate(files):
        header = some_file.split('_')
        some_class = header[-2]
        glrlm_index = header[-1].split('.')[0]

        # primeiras três colunas são a classe, a matriz à que pertence, e o tom de cinza, no intervalo [0, 63]
        df = pd.read_csv(os.path.join(args.path, some_file), index_col=[0, 1, 2])
        descriptor = GLRLMDescriptors(df)

        dict_data[some_class][glrlm_index] += [descriptor]
        train_data_raw += [[glrlm_mask[int(glrlm_index)]] + [getattr(descriptor, d) for d in columns] + [some_class]]

        for col in columns:
            limits[col]['min'] = min(limits[col]['min'], eval('descriptor.%s' % col))
            limits[col]['max'] = max(limits[col]['max'], eval('descriptor.%s' % col))

        print(i, '/', len(files))

    train_data = pd.DataFrame(train_data_raw, columns=['mask'] + columns + ['class'])
    train_data.to_csv('glrlm_training_set.csv', index=False)

    for m, dm in zip(glrlm, glrlm_mask):
        fig, axes = plt.subplots(len(class_names), len(columns), figsize=(16, 10))
        axes = axes.ravel()
        counter_axes = 0
        for c in class_names:
            axes[counter_axes].set_ylabel(c)
            for descriptor in columns:
                axes[counter_axes].hist(eval('np.array([x.%s for x in dict_data[c][m]])' % descriptor), bins=30, range=(limits[descriptor]['min'], limits[descriptor]['max']))
                axes[counter_axes].set_title('%s %s' % (descriptor, dm))
                counter_axes += 1

        plt.subplots_adjust(hspace=0.3)

        plt.savefig('glrlm_%s.pdf' % dm, format='pdf')


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
