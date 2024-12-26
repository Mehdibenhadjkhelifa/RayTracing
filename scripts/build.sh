if [ -d build ]
then
    ninja -C build
else
    echo "No generated build files to use"
    echo "Run scripts/setup.sh first"
fi
