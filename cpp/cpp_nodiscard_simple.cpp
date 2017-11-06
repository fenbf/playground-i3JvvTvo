
// just a return value for a single function:
[[nodiscard]] int Compute() { return 0; }

// mark the whole type:
[[nodiscard]] struct SuperImportantType { };
SuperImportantType CalcSuperImportant() { 
	return SuperImportantType();
}
SuperImportantType OtherFoo() { 
	return SuperImportantType();
}
SuperImportantType CalcType() { 
	return SuperImportantType();
}

int main()
{
    Compute();
	
	OtherFoo();
}