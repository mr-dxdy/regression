require 'spec_helper'

describe Regression do
  RSpec::Matchers.define :have_elements_as do |expected|
    match do |actual|
      status = true

      for i in 0..expected.length - 1
        if (actual[i] - expected[i]).abs > 0.001
          status = false
          break
        end
      end

      status
    end
  end

  subject { Regression }

  context 'example from matlab' do
    # see http://matlab.exponenta.ru/spline/book1/7.php

    let(:x_array) do
      [0.1, 0.3, 0.45, 0.5, 0.79, 1.1, 1.89, 2.4, 2.45]
    end

    let(:y_array) do
      [-3, -1, 0.9, 2.4, 2.5, 1.9, 0.1, -1.3, -2.6]
    end

    [
      [ 1, [0.675, -0.619] ],
      [ 3, [-4.5273, 17.0969, -12.1553, 2.2872] ],
      [ 5, [-3.8631, 4.7121, 35.9698, -62.4220, 33.9475, -6.0193] ]

    ].each do |order, result|

      it "when order is #{order}" do
        expect(subject.polyfit(x_array, y_array, order)).to have_elements_as(result)
      end
    end
  end
end
