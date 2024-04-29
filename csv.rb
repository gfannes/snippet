require 'csv'

csv = CSV.open('test.csv', 'wb', col_sep: '|')
csv << %w[A B C]
csv << %w[a b c]

csv.close()
