docker run -it                   \
		   --rm                  \
		   -e COLUMNS= -e LINES= \
		   -v $PWD:/ft_ping      \
		   ft_ping			     \
		   bash
