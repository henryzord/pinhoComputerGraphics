import argparse
import os
from PIL import Image


def main(args):
	n_files = len(os.listdir(args.path))

	for i, file in enumerate(os.listdir(args.path)):
		print('on file', i + 1, '/', n_files)
		f_img = os.sep.join([args.path, file])
		img = Image.open(f_img)
		img.convert('RGB')  # remove alpha channel
		img = img.resize((640, 640))  # redimensiona para 640x640 pixels
		img.save(f_img.split('.')[0] + '.bmp', format='bmp')


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
