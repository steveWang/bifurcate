all:
	 ghc src/bifurcate.hs -o ./bifurcate -threaded
clean:
	rm -f bifurcate
