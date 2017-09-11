all:	test

clean:
	make -C Debug clean
	rm -f out/*
	rm -f challenge

compile:
	make -C Debug all
#---

challenge : compile
	@cp Debug/challenge .

elephants.ts :
	wget http://test.unified-streaming.com/elephants.ts


test: elephants.ts challenge
	@echo "Testing"
	@mkdir -p out/ ; rm -f out/* ;  ./challenge elephants.ts
	@echo "Generated file kind " ; file out/*
	@echo " "
	@echo "Note: Using the ffplay command line tool the stream 33 and 34 are viewable/playable " 
