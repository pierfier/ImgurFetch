mkdir man
mkdir man/META-INF
mkdir man/OEBPS
mkdir man/images
./imgurfetch -c
cd man
zip -X0 man.zip mimetype
zip -rDX9 man.zip * -x mimetype
mv man.zip ../man.epub
cd ..
