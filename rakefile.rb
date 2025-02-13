my = Class.new do
  def initialize
    @here = Dir.pwd

    # We recreate '.clangd' each time: it contains the absolute path to @here
    create_clangd_
  end

  def default(what, extra = nil)
    case what
    when :filter then '*'
    when :settings
      language = extra
      case language
      when :cpp then 'clang:release:O3:c++23:PIC:64'
      when :zig then ''
      when :rust then ''
      when :dot then ''
      when :crystal then ''
      else raise("Unknown language '#{language}'")
      end
    else raise("Unknow what '#{what}'")
    end
  end

  def build_cpp(src_fn, settings)
    config = { options: [], defines: [], includes: [@here] }
    (settings || default(:settings, :cpp)).split(':').each do |setting|
      case setting
      when 'gcc' then config[:compiler] = 'g++'
      when 'clang'
        config[:compiler] = 'clang++'
        config[:options] << 'stdlib=libc++' # Necessary for std::expected with clang++. Make sure you have the latest libcxx and libcxx-devel installed.
      when 'release' then config[:defines] << 'NDEBUG'
      when 'debug' then config[:options] << 'g'
      when 'c++23' then config[:options] << 'std=c++23'
      when 'c++20' then config[:options] << 'std=c++20'
      when 'c++14' then config[:options] << 'std=c++14'
      when 'O3' then config[:options] << 'O3'
      when 'PIC' then config[:options] << 'fPIC'
      when '64' then config[:options] << 'm64'
      when '32' then config[:options] << 'm32'
      else raise("Unknow setting '#{setting}'")
      end
    end

    compiler = config[:compiler]
    options_str = config[:options].map { |option| "-#{option}" } * ' '
    defines_str = config[:defines].map { |define| "-D #{define}" } * ' '
    includes_str = config[:includes].map { |incl| "-I #{incl}" } * ' '

    exe_fn = "#{src_fn}.exe"

    Rake.sh("#{compiler} #{options_str} #{defines_str} #{src_fn} -o #{exe_fn} #{includes_str}")

    exe_fn
  end

  def build_zig(src_fn, _settings)
    exe_fn = "#{src_fn}.exe"

    Rake.sh("zig test #{src_fn} --test-no-exec -femit-bin=#{File.basename(exe_fn)}")

    exe_fn
  end

  def build_rust(src_fn, settings)
    (settings || default(:settings, :rust)).split(':').each do |setting|
    end

    exe_fn = "#{src_fn}.exe"

    Rake.sh("rustc -C opt-level=3 #{src_fn} -o #{exe_fn}")

    exe_fn
  end

  def build_crystal(cr_fn, settings)
    options = []
    (settings || default(:settings, :crystal)).split(':').each do |setting|
      case setting
      when 'release' then options << 'release'
      when 'static' then options << 'static'
      else raise("Unknow setting '#{setting}'")
      end
    end

    exe_fn = "#{cr_fn}.exe"
    Rake.sh("crystal build #{cr_fn} -o #{exe_fn} #{options.map { |option| "--#{option}" } * ' '}")
    exe_fn
  end

  def build_dot(dot_fn)
    pdf_fn = "#{dot_fn}.pdf"
    Rake.sh("dot -T pdf -o #{pdf_fn} #{dot_fn}")
    pdf_fn
  end

  def run(fn)
    start = Time.now
    case File.extname(fn)
    when '.exe' then Rake.sh("#{fn}")
    when '.pdf' then Rake.sh("evince #{fn}")
    else raise("Don't know how to run '#{fn}'")
    end
    duration = Time.now - start
    puts("Duration: #{duration}\n\n")
  end

  def filenames(ext:, filter: nil, verbose: false)
    fns = FileList.new(File.join(@here, "**/#{filter || default(:filter)}*.#{ext}")).to_a
    if verbose
      puts("Found #{fns.size} matching files:")
      fns.each do |fn|
        puts("- #{fn}")
      end
      puts
    end
    fns
  end

  private

  def create_clangd_
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
        	Add: [-xc++, -std=c++23, -stdlib=libc++, -DUTIL_LOG=0, -DL]

        ---

        If:
        	PathMatch: [.*\.cpp]
        CompileFlags:
        	Add: [-xc++, -std=c++23, -stdlib=libc++]

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
end.new

task :default do
  sh('rake -T')
end

desc("Build and run C++ snippet: [filter: #{my.default(:filter)}, settings: #{my.default(:settings, :cpp)}]")
task :cpp, %i[filter settings] do |_t, args|
  src_fns = my.filenames(filter: args[:filter], ext: :cpp, verbose: true)

  src_fns.each do |src_fn|
    exe_fn = my.build_cpp(src_fn, args[:settings])
    my.run(exe_fn)
  end
end

desc("Build and run Zig snippet: [filter: #{my.default(:filter)}, settings: #{my.default(:settings, :zig)}]")
task :zig, %i[filter settings] do |_t, args|
  src_fns = my.filenames(filter: args[:filter], ext: :zig, verbose: true)

  src_fns.each do |src_fn|
    exe_fn = my.build_zig(src_fn, args[:settings])
    my.run(exe_fn)
  end
end

desc("Build and run Rust snippet: [filter: #{my.default(:filter)}, settings: #{my.default(:settings, :rust)}]")
task :rust, %i[filter settings] do |_t, args|
  src_fns = my.filenames(filter: args[:filter], ext: :rs, verbose: true)

  src_fns.each do |src_fn|
    exe_fn = my.build_rust(src_fn, args[:settings])
    my.run(exe_fn)
  end
end

desc("Build and run Crystal snippet: [filter: #{my.default(:filter)}, settings: #{my.default(:settings, :crystal)}]")
task :cr, %i[filter settings] do |_t, args|
  src_fns = my.filenames(filter: args[:filter], ext: :cr, verbose: true)

  src_fns.each do |src_fn|
    exe_fn = my.build_crystal(src_fn, args[:settings])
    my.run(exe_fn)
  end
end

desc("Compile and show dot snippet: [filter: #{my.default(:filter)}, settings: #{my.default(:settings, :dot)}]")
task :dot, %i[filter settings] do |_t, args|
  src_fns = my.filenames(filter: args[:filter], ext: :dot, verbose: true)

  src_fns.each do |src_fn|
    pdf_fn = my.build_dot(src_fn)
    my.run(pdf_fn)
  end
end

desc('Clean')
task :clean do
  %w[exe dwarf o].each do |ext|
    rm_f(FileList.new("**/*.#{ext}"))
  end
end
