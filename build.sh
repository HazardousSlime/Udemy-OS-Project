export LANG=POSIX			#fix filename encoding?
export LC_ALL=
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
make all
