DATA_DIR="/mnt/tmpssd/aikv/ycsb-data-test"
echo "CONVERTING RMI MODEL..."
python3 convert_rmi.py "${DATA_DIR}/raw_prefill.dat" "/lab505/gjk/viper/index/RMI/RMI-package/data_200M_uint64"
cd "/lab505/gjk/viper/index/RMI/RMI-package"
cargo run --release -- data_200M_uint64 my_first_rmi linear,linear 100
cd "/lab505/gjk/viper"
./redo.sh