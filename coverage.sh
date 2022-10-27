read -p "Do you want to delete 'build' and all files contained? (Y/N) " -n 1 -r
if [[ ! $REPLY =~ ^[Nn]$ ]]
then
    printf "\n"
    printf "DELETING"
    rm -r build
    mkdir build
fi

printf "\n"
cd build

read -p "Compile? (Y/N) " -n 1 -r
if [[ ! $REPLY =~ ^[Nn]$ ]]
then
    cmake ..
    cmake --build .
fi
printf "\n"
read -p "Execute TESTS? (Y/N) " -n 1 -r
if [[ ! $REPLY =~ ^[Nn]$ ]]
then
    lcov --capture --initial  --directory . --output-file base.info
    g++ -o test_node -fprofile-arcs -ftest-coverage ../test/test_node.cpp ../src/node.cpp 
    ./test_node || exit_code=$?
    lcov  --capture  --directory . --output-file test_node.info
    g++ -o test_link -fprofile-arcs -ftest-coverage ../test/test_link.cpp ../src/link.cpp 
    ./test_link || exit_code=$?
    lcov  --capture  --directory . --output-file test_link.info
    g++ -o test_network -fprofile-arcs -ftest-coverage ../test/test_network.cpp ../src/network.cpp ../src/node.cpp ../src/link.cpp
    ./test_network || exit_code=$?
    lcov  --capture  --directory . --output-file test_network.info
    g++ -o test_random_variable -fprofile-arcs -ftest-coverage ../test/test_random_variable.cpp ../src/random_variable.cpp ../src/exp_variable.cpp
    ./test_random_variable || exit_code=$?
    lcov  --capture  --directory . --output-file test_random_variable.info
    g++ -o test_controller -fprofile-arcs -ftest-coverage ../test/test_controller.cpp ../src/controller.cpp ../src/network.cpp ../src/node.cpp ../src/link.cpp ../src/connection.cpp ../src/bitrate.cpp  ../src/allocator.cpp
    ./test_controller || exit_code=$?
    lcov  --capture  --directory . --output-file test_controller.info
    g++ -o test_bitrate -fprofile-arcs -ftest-coverage ../test/test_bitrate.cpp ../src/bitrate.cpp 
    ./test_bitrate || exit_code=$?
    lcov  --capture  --directory . --output-file test_bitrate.info
    g++ -o test_simulator -fprofile-arcs -ftest-coverage ../test/test_simulator.cpp ../src/simulator.cpp  ../src/controller.cpp ../src/network.cpp ../src/node.cpp ../src/link.cpp ../src/connection.cpp ../src/bitrate.cpp ../src/allocator.cpp ../src/event.cpp ../src/uniform_variable.cpp ../src/random_variable.cpp ../src/exp_variable.cpp 
    ./test_simulator || exit_code=$?
    lcov  --capture  --directory . --output-file test_simulator.info
    lcov --add-tracefile test_node.info --add-tracefile test_link.info --add-tracefile test_network.info --add-tracefile test_random_variable.info --add-tracefile test_controller.info --add-tracefile test_bitrate.info --add-tracefile test_simulator.info --output-file total.info
    lcov --remove total.info '*/c++/*' '*test*' '*.hpp' -o final.info
    mkdir ../public/coverage-details
    genhtml --prefix .  --ignore-errors source final.info --legend --title "commit SHA1" --output-directory=./public/coverage-details
    printf "\n"
fi
printf "\n"

read -p "Create BACKUP in DRIVE? (Y/N) " -n 1 -r
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    printf "\n"
    exit 1
fi
