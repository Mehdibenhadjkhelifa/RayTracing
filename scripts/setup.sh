if ! [ -d build ]  
then
    mkdir ./build
fi
if [ $# -gt 0 ]
then
    if [[ $1 = "Debug" || $1 = "Release" ]];
    then
        echo "$1 has been chosen as build type"
        buildtype=$1
    else
        echo "Unvalid build type, Choose either debug-release"
    fi
    cmake -S . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=$buildtype -G Ninja && 
    echo "Project Setup Successfully" || echo "Project Failed to setup"
else
    echo "No build type selected,Opting for default configuration"
    echo "Script usage: $0 Debug/Release"
    cmake -S . -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug -G Ninja && 
    echo "Project Setup Successfully" || echo "Project Failed to setup"
fi
