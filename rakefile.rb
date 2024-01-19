my = Class.new() do
    def initialize()
        @here = Dir.pwd()
    end

    def build(src_fn, settings)
        default_settings = 'release:O3:c++20'

        config = {options: [], defines: [], includes: [@here]}
        (settings || default_settings).split(':').each do |setting|
            case setting
            when 'release' then config[:defines] << 'NDEBUG'
            when 'debug' then config[:options] << 'g'
            when 'c++20' then config[:options] << 'std=c++20'
            when 'O3' then config[:options] << 'O3'
            else raise("Unknow setting '#{setting}'")
            end
        end

        options_str = config[:options].map{|option|"-#{option}"}*' '
        defines_str = config[:defines].map{|define|"-D #{define}"}*' '
        includes_str = config[:includes].map{|incl|"-I #{incl}"}*' '

        exe_fn = "#{src_fn}.exe"

        Rake.sh("g++ #{options_str} #{defines_str} #{src_fn} -o #{exe_fn} #{includes_str}")

        exe_fn
    end

    def run(exe_fn)
        start = Time.now()
        Rake.sh("#{exe_fn}")
        duration = Time.now()-start

        puts("Duration: #{duration}\n\n")
    end

    def filenames(filter: nil, ext: )
        FileList.new(File.join(@here, "*#{filter}*.#{ext}")).to_a()
    end
end.new()

task :default do
    sh('rake -T')
end

desc('Build and run C++ snippe [filter: glob, settings: release:debug:c++20:O3]')
task :cpp, %i[filter settings] do |t, args|
    my.filenames(filter: args[:filter], ext: :cpp).each do |src_fn|
        exe_fn = my.build(src_fn, args[:settings])
        my.run(exe_fn)
    end
end

desc('Clean')
task :clean do
    rm_f(FileList.new("*.exe"))
end

