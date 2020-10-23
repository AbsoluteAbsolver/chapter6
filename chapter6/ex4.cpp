// excerise 4 of chpater 6
//Write a program where you first enter a set of name - and -value pairs, such
//as Joe 17 and Barbara 22. For each pair, add the name to a vector called
//namesand the number to a vector called scores(in corresponding positions,
//	so that if names[7] == "Joe" then scores[7] == 17).Terminate input
//	with NoName 0. Check that each name is unique and terminate with an
//	error message if a name is entered twice.Write out all the(name, score)
//	pairs, one per line.
//Define a class Name_value that holds a string and a value. Rework exercise
//19 in Chapter 4 to use a vector<Name_value> instead of two vectors.

class Name_value {
public:
	string nameval;
	int val;
	Name_value(string name, int score)
		:nameval(name), val(score) {}
};

int main() 
try
{
	vector<Name_value> names;
	while ((cin >> name >> score) && (name != 'Noname' || score != 0)) {
		if (!cin) error('incorrect input');
	}
}

catch (exception& e) {
	cerr << "error: " << e.what() << '\n';
	keep_window_open();
	return 1;
}
catch (...) {
	cerr << "Oops: unknown exception!\n";
	keep_window_open();
	return 2;
}