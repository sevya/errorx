from setuptools import setup, find_packages

setup(name='ErrorX',
	version='1.0',
	description='ErrorX DNA sequence correction software',
	author='Alex Sevy',
	author_email='alex.sevy@gmail.com',
	packages=['errorx'],
#	install_requires=['biopython', 'tqdm'],
	#package_dir=
	package_data={'errorx': ['ErrorXOptions.py','errorx_lib.so','bin/igblastn','database/human/*','database/mouse/*','internal_data/human/*','internal_data/mouse/*','optional_file/*']},
	include_package_data = True)
