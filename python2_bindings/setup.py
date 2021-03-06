from setuptools import setup, find_packages

setup(name='ErrorX',
	version='1.1.0',
	description='ErrorX DNA sequence correction software',
	author='Alex Sevy',
	author_email='alex@endeavorbio.com',
	packages=['errorx'],
	package_data=
		{'errorx': 
			['ErrorXOptions.py',
			 'errorx_lib.pyd',
			 'errorx_lib.so',
			 'model.nnet',
			 'bin/*',
			 'database/Ig/human/*',
			 'database/TCR/human/*',
			 'database/Ig/mouse/*',
			 'database/TCR/mouse/*',
			 'internal_data/human/*',
			 'internal_data/mouse/*',
			 'optional_file/*']
		},
	include_package_data = True)
