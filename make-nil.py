library_name = "nil"

import os
import sys
import time

def get_last_modification(path):
	try:
		os.stat(path).st_mtime
	except:
		return False
		
def get_extension(file_name):
	offset = file_name.find(".")
	if offset == -1:
		return False
	else:
		return file_name[offset + 1 : ]
		
def get_latest_timestamp(files):
	latest_timestamp = False
	for file_name in files:
		timestamp = get_last_modification(file_name)
		if latest_timestamp == False or (timestamp != False and timestamp > latest_timestamp):
			latest_timestamp = timestamp
	return latest_timestamp
	
def hpp_filter(file_name):
	return get_extension(file_name) == "hpp"
	
def cpp_filter(file_name):
	return get_extension(file_name) == "cpp"

library_directory = library_name

output_directory = library_directory + "/object"
library_name = "lib" + library_name + ".a"

try:
	os.mkdir(output_directory)
except:
	pass

if len(sys.argv) >= 2:
	offset = int(sys.argv[1])
else:
	offset = 0

object_list = " "

files = os.listdir(library_directory)
hpp_files = filter(hpp_filter, files)
cpp_files = filter(cpp_filter, files)

object_files = os.listdir(output_directory)

hpp_timestamp = get_latest_timestamp(hpp_files)
cpp_timestamp = get_latest_timestamp(cpp_files)
object_timestamp = get_latest_timestamp(object_files)

recompile_all = (object_timestamp != False and hpp_timestamp != False and hpp_timestamp > object_timestamp)

i = 1
for file_name in files:
	if len(file_name) < 4:
		continue
	extension = get_extension(file_name)
	if extension == False or extension != "cpp":
		continue
	base = file_name[0 : - (len(extension) + 1)]
	
	object_path = output_directory + "/" + base + ".o"
	
	if recompile_all == False:
		cpp_time = get_last_modification(file_name)
		object_time = get_last_modification(object_path)
		
		if cpp_time == False:
			print "Unable to retrieve timestamp for " + file_name
			continue
		
		if object_time != False and cpp_time <= object_time:
			print "Skipping " + file_name
			continue
	
	command = "g++ -c -g -I . " + library_directory + "/" + base + ".cpp -o " + object_path
	object_list += " " + object_path
	if offset == 0 or i == int(offset):
		print "Executing " + command
		os.system(command)
	i += 1
	
try:
	os.remove(library_name)
except:
	pass

if offset == 0:
	command = "ar cq " + library_name + object_list
	print "Executing " + command
	os.system(command)