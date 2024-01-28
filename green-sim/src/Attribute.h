#pragma once



struct Attribute {


	// Generates the gl objects associated with the attribute
	void Generate();

	// This function changes the attribute's values by the values in D
	// Useful for delta
	void Modify(Attribute D);

	// To be called by Process on runtime loop
	// ie: Meant to do opengl shenanigans
	// Future expansion?: batching -> define new functions to stage OnUpdate type
	//		of changes and then allow a single OnUpdate (overload) call that will
	//		issue for example: a single gl call to GPU.
	void OnUpdate();
};