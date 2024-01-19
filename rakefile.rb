my = Class.new() do
    def initialize()
        @here = Dir.pwd()

        # We recreate '.clangd' each time: it contains the absolute path to @here
        create_clangd_()
    end

    def default(what, extra = nil)
        case what
        when :filter then '*'
        when :settings
            language = extra
            case language
            when :cpp then 'gcc:release:O3:c++20:PIC:64'
            else raise("Unknown language '#{language}'")
            end
        else raise("Unknow what '#{what}'")
        end
    end

    def build(src_fn, settings)
        config = {options: [], defines: [], includes: [@here]}
        (settings || default(:settings, :cpp)).split(':').each do |setting|
            case setting
            when 'gcc' then config[:compiler] = 'g++'
            when 'clang' then config[:compiler] = 'clang++'
            when 'release' then config[:defines] << 'NDEBUG'
            when 'debug' then config[:options] << 'g'
            when 'c++20' then config[:options] << 'std=c++20'
            when 'O3' then config[:options] << 'O3'
            when 'PIC' then config[:options] << 'fPIC'
            when '64' then config[:options] << 'm64'
            when '32' then config[:options] << 'm32'
            else raise("Unknow setting '#{setting}'")
            end
        end

        compiler = config[:compiler]
        options_str = config[:options].map{|option|"-#{option}"}*' '
        defines_str = config[:defines].map{|define|"-D #{define}"}*' '
        includes_str = config[:includes].map{|incl|"-I #{incl}"}*' '

        exe_fn = "#{src_fn}.exe"

        Rake.sh("#{compiler} #{options_str} #{defines_str} #{src_fn} -o #{exe_fn} #{includes_str}")

        exe_fn
    end

    def run(exe_fn)
        start = Time.now()
        Rake.sh("#{exe_fn}")
        duration = Time.now()-start

        puts("Duration: #{duration}\n\n")
    end

    def filenames(filter: nil, ext: )
        FileList.new(File.join(@here, "**/#{filter || default(:filter)}*.#{ext}")).to_a()
    end

    private
    def create_clangd_()
        File.open('.clangd', 'w') do |fo|
            fo.puts <<~EOS
Style:
    IncludeDelimiter: AlwaysBrackets
CompileFlags:
    Add: [-I#{@here}]

---

If:
	PathMatch: [.*\.hpp]
CompileFlags:
	Add: [-xc++, -std=c++20, -DUTIL_LOG=0, -DL]

---

If:
	PathMatch: [.*\.cpp]
CompileFlags:
	Add: [-xc++, -std=c++20]

---

If:
	PathMatch: [.*\.h]
CompileFlags:
	Add: [-xc, -std=c17]

---

If:
	PathMatch: [.*\.c]
CompileFlags:
	Add: [-xc, -std=c17]
            EOS
        end
    end
end.new()

task :default do
    sh('rake -T')
end

desc("Build and run C++ snippet: [filter: #{my.default(:filter)}, settings: #{my.default(:settings, :cpp)}]")
task :cpp, %i[filter settings] do |t, args|
    src_fns = my.filenames(filter: args[:filter], ext: :cpp)
    puts("Found #{src_fns.size()} matching files:")
    src_fns.each do |src_fn|
        puts("- #{src_fn}")
    end
    puts()
    
    src_fns.each do |src_fn|
        exe_fn = my.build(src_fn, args[:settings])
        my.run(exe_fn)
    end
end

desc('Clean')
task :clean do
    rm_f(FileList.new("**/*.exe"))
end

