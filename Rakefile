require 'lib/regression/version'
require 'rake/extensiontask'

spec = Gem::Specification.new 'regression', Regression::VERSION do |s|
  s.authors = ["German Antsiferov"]
  s.email   = ["dxdy@bk.ru"]
  s.summary = %q{This project implements a simple least-squares polynomial fit}
  s.license = "MIT"

  s.extensions = %w[ext/regression/extconf.rb]

  s.files = %w[
    LICENSE.txt
    Rakefile
    lib/regression.rb
    lib/regression/version.rb
    ext/regression/extconf.rb
    ext/regression/regression.c
  ]

  s.add_development_dependency "rake-compiler", "~> 0.8"
  s.add_development_dependency "rspec", "~> 3.3"
end

Gem::PackageTask.new(spec) do |pkg|
end

Rake::ExtensionTask.new('regression', spec) do |ext|
  ext.lib_dir = "lib/regression"
end

task :default => [:compile]
