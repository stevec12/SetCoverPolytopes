# usage: cleancoverIE.sh [Size of Universe] [Number of Sets] [filename-stem]
# Requires Polymake
# Generates set covers, enumerates the solutions, plugs them into Polymake to generate the facets.
# Set Cover Problems such that
# > No trivial solution, no subset Si=U
# > No empty subsets
# > No duplicate subsets
# > Full dimensional problem
