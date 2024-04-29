require "file_utils"

puts File::MatchOptions.glob_default()
puts File::MatchOptions::None

content = File.read("file_io.cr")
puts content

exts = %w[hpp cpp h c rb md chai graphml py]

ext__files = Hash(String, Array(String)).new{|h, k|h[k] = Array(String).new()}
FileUtils.cd("/home/geertf/am") do
  files = Dir.glob(exts.map{|ext|"**/*.#{ext}"}, match: File::MatchOptions::None)
  files.each do |file|
    ext__files[File.extname(file)] << file
  end
end

ext__files.each do |ext, files|
  puts("#{ext}\t#{files.size}")
end
