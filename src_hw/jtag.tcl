set_param messaging.defaultLimit 10000
10000




# MEMORY_READER 0
[create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030000 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030004 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030008 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4003000C -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030010 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030014 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030018 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4003001C -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030020 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030024 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030028 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4003002C -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030030 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030034 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40030038 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4003003C -type read -force
run_hw_axi read_registry]


0x40030000: 0201d320
0x40030004: 000003e8
0x40030008: 00000008
0x4003000C: 00000000
0x40030010: 00000000
0x40030014: 00000102
0x40030018: 00000000
0x4003001C: 0bebc200
0x40030020: 001e8480
0x40030024: 00000000
0x40030028: 00000008
0x4003002C: 1f2a8372
0x40030030: 00000000
0x40030034: 00000206
0x40030038: 00000000
0x4003003C: 00000000



#reset
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40030000 -data 0x00005302 -type write -force
run_hw_axi write_registry


#enable single request  
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40030000 -data 0x00005308 -type write -force
run_hw_axi write_registry

#calibration
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40030000 -data 0x00005320 -type write -force
run_hw_axi write_registry

#calibration
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40030000 -data 0x00005322 -type write -force
run_hw_axi write_registry


#calibration count set
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40030008 -data 0x00000010 -type write -force
run_hw_axi write_registry




#start measure on dev
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x4004002C -data 0x0000080F -type write -force
run_hw_axi write_registry


#calibration
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40030000 -data 0x00005322 -type write -force
run_hw_axi write_registry

# device reading registers
[create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040000 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4004001C -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040020 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040024 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040028 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4004002C -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040030 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040034 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040038 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4004003C -type read -force
run_hw_axi read_registry]


[create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x4004001C -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040020 -type read -force
run_hw_axi read_registry]
[create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040030 -type read -force
run_hw_axi read_registry
create_hw_axi_txn read_registry [get_hw_axis hw_axi_1] -address 0x40040034 -type read -force
run_hw_axi read_registry]

create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x4004001D -data 0x00000400 -type write -force
run_hw_axi write_registry
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40040020 -data 0x01010100 -type write -force
run_hw_axi write_registry
create_hw_axi_txn write_registry [get_hw_axis hw_axi_1] -address 0x40040028 -data 0x00000700 -type write -force
run_hw_axi write_registry


