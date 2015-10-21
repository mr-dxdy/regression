Regression
=========

## Installation

```bash
rake gem
```

## Usage

```ruby
x = [0.1, 0.3, 0.45, 0.5, 0.79, 1.1, 1.89, 2.4, 2.45]
y = [-3, -1, 0.9, 2.4, 2.5, 1.9, 0.1, -1.3, -2.6]

order = 1

Regression.polyfit(x, y, order)
# => [0.675454830824306, -0.619147643027931]
```
