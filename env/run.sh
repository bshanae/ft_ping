docker run -it                   \
		   --rm                  \
		   -e COLUMNS= -e LINES= \
		   -v $PWD:/ft_ping      \
		   --network host        \
		   ft_ping			     \
		   bash
