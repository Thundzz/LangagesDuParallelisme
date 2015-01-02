#!/usr/bin/env ruby

d_alphabet="abcdefghijklmnopqrstuvwxyz"
d_nbp = 5
d_nbt = 1
d_depth= 4

def try_find_password(password, options = {})

	d_alphabet="abcdefghijklmnopqrstuvwxyz"
	d_nbp = 5
	d_nbt = 1
	d_depth= 4
	options = {alphabet: d_alphabet, nbp: d_nbp, nbt: d_nbt, depth: d_depth}.merge(options)
	cmd = "mpiexec -np 1 ./master.out  #{options[:nbp]} "+
								       "#{options[:nbt]} "+
								       "#{options[:alphabet]} "+
								       "#{options[:depth]} "+
								       "#{password}"
	puts cmd
	system(cmd)
end


try_find_password("zizi")
try_find_password("zizou")