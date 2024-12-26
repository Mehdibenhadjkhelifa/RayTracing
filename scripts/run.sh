if [ -d build ]
then
    if [ -d build/bin ]
    then
        build/bin/RayTracing || echo "Failed to run the project"
    else
        echo "No binary available"
        echo "Build the project first using scripts/build.sh"
    fi
else
    echo "No build files or binary available"
    echo "Run scripts/setup.sh then scripts/build.sh before running the project"
fi
