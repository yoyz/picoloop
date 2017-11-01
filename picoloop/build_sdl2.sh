if [ ! -f SDL2-2.0.7.tar.gz ] 		; then wget https://www.libsdl.org/release/SDL2-2.0.7.tar.gz ; fi
if [ ! -f SDL2_image-2.0.2.tar.gz ] 	; then wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.2.tar.gz ; fi
if [ ! -f SDL2_ttf-2.0.14.tar.gz ] 	; then wget https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.14.tar.gz ; fi

rm -rf SDL2-2.0.7 SDL2_image-2.0.2 SDL2_ttf-2.0.14
DEST=~/local/SDL2-clang/
tar xzvf SDL2-2.0.7.tar.gz 
cd SDL2-2.0.7/
./configure CC=clang --prefix $DEST
make
make install
cd ..

tar xzvf SDL2_image-2.0.2.tar.gz 
cd SDL2_image-2.0.2/
( export PATH=$PATH:~/local/SDL2-clang/bin/  ; ./configure CC=clang --prefix $DEST  )
make install
cd ..

( export PATH=$PATH:~/local/SDL2-clang/bin/  ; ./configure CC=clang --prefix $DEST  )
make
make install
cd ..

