h = {} of String => Array(Int32)

h["abc"] ||= [] of Int32
h["abc"] << 123

puts h
