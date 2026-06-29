emnist/gzip.zip:
	mkdir -p emnist
	curl --output-dir emnist/compressed/ --remote-name https://biometrics.nist.gov/cs_links/EMNIST/gzip.zip

extract: emnist/gzip.zip
	{ cd emnist; unzip gzip.zip; }
