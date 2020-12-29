A client that downloads albums from imgur and compiles it into an epub file

The request to get an album: https://api.imgur.com/3/album/{id}/images

Notes for the user:
Imgur's API requires a client key.
This means that you have to create your own imgur account and get a key
directly from imgur.

The key and id must from come a file specified as a commandline argument

Compilation of the ePUB
=======================
- All of the images are downloaded in the indicated file
- It is up to the USER to determine what picture is to be the cover photo. It must be named "cover" plus the extension of the image.


Usage Notes
===========
- image filenames should just be the number order of which it is downloaded. Use this following command to remove any prefixes (in this case it is image):

for i in *; do
mv "$i" "`echo $i | sed "s/image//"`";
done
