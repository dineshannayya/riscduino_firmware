

1. Tar command
   tar -cvf riscduino-1.0.0.tar riscduino
2. Zip command
   gzip -f riscduino-1.0.0.tar
3. find sha256sum
   sha256sum riscduino-1.0.0.tar.gz
4. Update the installed.json, sha256 and version

# use sha1sum to define the version
   sha1sum.exe riscduino-1.0.2.tar.gz
