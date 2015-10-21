# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

spec = Gem::Specification.new do |spec|
  spec.name          = "regression"
  spec.version       = "0.0.1"
  spec.authors       = ["German Antsiferov"]
  spec.email         = ["dxdy@bk.ru"]

  spec.summary       = %q{This project implements a simple least-squares polynomial fit}
  spec.license       = "MIT"

  spec.files = Dir['{lib,ext}/**/*', 'LICENSE*']

  spec.add_development_dependency "rspec", "~> 3.3"
end
