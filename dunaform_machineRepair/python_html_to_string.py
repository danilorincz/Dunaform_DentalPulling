input_file_path = r"C:\projects\dunaform_machineRepair\dunaform_machineRepair\webpage.html"
output_file_path = r"C:\projects\dunaform_machineRepair\dunaform_machineRepair\webpage.h"

with open(input_file_path, 'r', encoding='utf-8') as f:
    content = f.read()

header_content = f'#pragma once\n\nconst char* webpageCode = R"=====(\n{content})=====";'

with open(output_file_path, 'w', encoding='utf-8') as f:
    f.write(header_content)
