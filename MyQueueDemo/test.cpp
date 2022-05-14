#include <iostream>

class test
{
private:
	int i;
	int c;
public:
	test(int n = 10, int j = 20);
	int getnumber() const;
};

test::test(int n, int j) : i(n),c(j)
{

}

int test::getnumber() const
{
	return i;
}

int main(void)
{
	test g;
	std::cout << g.getnumber() << std::endl;

	return 0;
}