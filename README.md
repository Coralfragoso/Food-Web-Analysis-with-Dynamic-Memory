# Food-Web-Analysis-with-Dynamic-Memory
Dynamic food web analysis in C with command-line modes. Uses Org struct (name, dynamic prey array) to manage predator-prey relationships. Features species expansion, extinction, and food source supplementation with manual reallocation (no realloc()). Analyzes apex predators, producers, flexible eaters, tastiest food, heights, and vore types.

Features

	Dynamic data structures: Organisms stored in dynamically allocated array of Org structs, each with dynamic prey index array
	
	Command-line modes:

		-b (basic mode): Prevents modifications after initial build
		
		-d (debug mode): Prints web state after each modification
		
		-q (quiet mode): Suppresses prompt messages

	Species expansion: Add new organisms with addOrgToWeb()
	
	Food source supplementation: Add predator-prey relationships with addRelationToWeb()
	
	Species extinction: Remove organisms with removeOrgFromWeb() (most challenging task)

	Comprehensive analysis:

		Print all organisms and their prey
		
		Identify apex predators (not prey of any organism)
		
		Identify producers (eat no other organisms)
		
		Find most flexible eaters (most prey)
		
		Find tastiest food (most predators)
		
		Calculate food web heights (longest path from producers)
		
		Categorize vore types (producer, herbivore, omnivore, carnivore)

Implementation

	Manual reallocation: malloc() new array, copy elements, free() old array
	
	Proper memory management with freeWeb() to prevent leaks
	
	Input redirection support with 24 predefined food web files
	
	Valgrind validation for memory leak detection

Sample Food Webs

	Simple, Grassland, Aquatic, Terrestrial, Another, Mixed
	
	Each with Basic, AddOne, OneExtinction, and ManyMods variants

Requirements

	No realloc() - manual reallocation only
	
	Exact memory allocation (no wasted space)
	
	Maintain input order for all displays
