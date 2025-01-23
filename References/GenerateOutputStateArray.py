# Function to format and print a binary number
def write_binary_strings_to_file(num_bits, count, file_name):
    with open(file_name, "w") as file:
        state_num = 0
        for i in range(count):
            # Generate the binary representation of the number
            binary_str = f"{i:0{num_bits}b}"  # Format the number as a binary string with leading zeros

            # account for logical linking of relay R06 with R05
            if (binary_str[-5] == '1') and (binary_str[-6] == '1'):
                continue
            elif (binary_str[-5] == '0') and (binary_str[-6] == '1'):
                # binary_str[-5] = '1'
                binary_str = binary_str[:num_bits-5] + '1' + binary_str[-5+1:]

            # Convert the string into the desired format
            formatted_str = "{" + ", ".join(binary_str) + "}"

            # Print the formatted string
            file.write(f'{formatted_str}, // state {state_num} \n')

            state_num += 1
    
    return state_num


# Parameters
num_bits = 9            # Number of binary digits
count = 2**num_bits     # Number of binary strings to generate (2^9)
file_name = "OutputStateArray.txt"

# Print the binary strings
total_states = write_binary_strings_to_file(num_bits, count, file_name)

print(f"Binary strings (count: {total_states}) written to: {file_name}")
