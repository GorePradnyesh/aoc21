import sys
import os
import pathlib

# Add Common to sys path
sys.path.append(os.path.join(pathlib.Path(__file__).parent.resolve(),'..', 'Common'))

import DataUtils

#####################

## constants
version_length = 3
type_length = 3
literal_digit_len = 5 # 4 + 1 for indication of last group

packet_type_literal = 4


#####################

class Packet:
    def __init__(self, version, type, processed_size) -> None:
        self.version = version
        self.type = type
        self.packet_size = processed_size + 3 + 3 # 3 bits each for version and type

class LiteralPacket(Packet):
    def __init__(self, version, value, processed_size) -> None:
        super().__init__(version, packet_type_literal, processed_size)
        self.value = value

    def __repr__(self) -> str:
        return f'L:{self.value} (v{self.version})'

    def get_version_sum(self):
        return self.version


class OperatorPacket(Packet):
    def __init__(self, version, type, processed_size, field_type, packets) -> None:
        super().__init__(version, type, processed_size)
        self.sub_packet_length = 0
        if field_type == 0:
            self.sub_packet_length = 15
        else:
            self.sub_packet_length = 11
        self.packets = packets
        self.packet_size = 3 + 3 + 1 + self.sub_packet_length + processed_size
        pass
    
    def __repr__(self) -> str:
        return f'O:{self.type}(v{self.version})->{self.packets}'

    def get_version_sum(self):
        version_sum = 0
        for packet in self.packets:
            version_sum += packet.get_version_sum()
        return version_sum + self.version

class MutableInput:
    def __init__(self, input) -> None:
        self.input = input

    def cleave_at_index(self, index):
        ret, self.input = self.input[:index], self.input[index:len(self.input)]
        return ret

####################


def get_version(mutable_input) -> int:
    version_str = mutable_input.cleave_at_index(version_length)
    version = int(version_str, 2)
    return version

def get_type(mutable_input) -> int:
    type_str = mutable_input.cleave_at_index(type_length)
    type = int(type_str, 2)
    return type

def process_literal(mutable_input, version):
    last_value = False
    final_bin_str = ''
    processed_size = 0
    while True:
        digit_str = mutable_input.cleave_at_index(literal_digit_len)
        processed_size += len(digit_str)
        if len(digit_str) < 5:
            print('Exiting. Unexpected length. Last digit not reached.')
            exit -1
        if digit_str[0] == '0':
            last_value = True
        final_bin_str += digit_str[1:literal_digit_len]
        if last_value:
            break
    return LiteralPacket(version, int(final_bin_str, 2), processed_size)

def process_operator_packet_len(mutable_input, process_length):
    processed_size = 0
    packets = []
    while processed_size < process_length:
        packet = process_binary(mutable_input)
        packets.append(packet)
        processed_size += packet.packet_size
    return packets

def process_operator_packet_count(mutable_input, process_count):
    packets = []
    for index in range(process_count):
        packet = process_binary(mutable_input)
        packets.append(packet)
    return packets

def process_operator(mutable_input, version, type):
    field_val = int(mutable_input.cleave_at_index(1), 2)
    len_field_size =  15 if field_val == 0 else 11
    process_value = int(mutable_input.cleave_at_index(len_field_size), 2)
    processed_size = 0
    packets = []
    if field_val == 1:
        packets = process_operator_packet_count(mutable_input, process_value)
    else:
        packets = process_operator_packet_len(mutable_input, process_value)
    processed_size = sum(packet.packet_size for packet in packets)
    return OperatorPacket(version, type, processed_size, field_val, packets)


def process_binary(mutable_input):
    ver = get_version(mutable_input)
    type = get_type(mutable_input)
    packet = None
    if type == packet_type_literal:
        packet = process_literal(mutable_input, ver)
    else:
        packet = process_operator(mutable_input, ver, type)
    return packet

####################

def hex_to_bin_str(raw_hex_str):
    binary_str = ''
    for hex_char in raw_hex_str:
        b_str = f'{int(hex_char, 16):#06b}'
        binary_str += b_str[2:]
    return binary_str

def process(input_file_name):
    input_path = os.path.join(pathlib.Path(__file__).parent.resolve(), 'input')    
    filePath = os.path.join(input_path, input_file_name)

    with open(filePath) as fp:
        raw_hex_string = fp.readline()
        binary_string = hex_to_bin_str(raw_hex_string.strip())
        mutable_input = MutableInput(binary_string)
        packet = process_binary(mutable_input)

    print(f'Sum:{packet.get_version_sum()} for Packet: {packet}, ')
    
process('z3.txt')