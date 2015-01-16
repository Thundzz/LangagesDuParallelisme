#!/usr/bin/env ruby

def try_find_password(password, options = {})
	d_alphabet="abcdefghijklmnopqrstuvwxyz"
	d_nbp = 4
	d_nbt = 2
	d_depth= 5
	options = {alphabet: d_alphabet, nbp: d_nbp, nbt: d_nbt, depth: d_depth}.merge(options)
	cmd = "mpiexec -np 1 ./master.out  #{options[:nbp]} "+
								       "#{options[:nbt]} "+
								       "#{options[:alphabet]} "+
								       "#{options[:depth]} "+
								       "#{password}"
	puts cmd
	system(cmd)
end


def launch_bench_procs(max_procs)
	nb_iter = 3
	d_alphabet="abcdefghijklmnopqrstuvwxyz"
	d_nbt = 2
	d_depth = 6
	time = Array.new(max_procs, 0.0)
	iter_time = 0.0
	(2..max_procs).each do |d_nbp|
		for k in 1..nb_iter
			options = {}
			password = gen_random_pwd(d_depth)
			options = {alphabet: d_alphabet, nbp: d_nbp, nbt: d_nbt, depth: d_depth}.merge(options)
			cmd = "mpiexec -np 1 ./master.out  #{options[:nbp]} "+
										       "#{options[:nbt]} "+
										       "#{options[:alphabet]} "+
										       "#{options[:depth]} "+
										       "#{password}"
			puts cmd
			system(cmd)
			
			File.open("time_#{d_nbp}_#{d_nbt}_#{d_depth}.dat", "r") do |file|
				file.each_line do |line|
					time[d_nbp] = line.to_s.split(" ")[0].to_f + time[d_nbp].to_f
				end
			end
		end
	end

	seq = time[2]
	for i in 2..max_procs
		time[i] = seq/time[i]
	end
	File.open("speedup.dat", "w") do |file|
		for k in 2..max_procs
			file.puts("#{k} #{time[k]}")
		end
	end
end

def gen_random_pwd(length)
	chars = 'abcdefghijklmnopqrstuvwxyz'
	password = ''
	length.times { password << chars[rand(chars.size)] }
	password
end

# try_find_password("zaza")
# try_find_password("zizou")
# puts gen_random_pwd 12
launch_bench_procs 8
