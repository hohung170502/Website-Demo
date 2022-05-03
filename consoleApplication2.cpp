// C++ program to check if two trees have
// same structure
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

struct Node
{
	int data;
	struct Node* left;
	struct Node* right;
};

Node* newNode(int data)
{
	Node* node = new Node;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return(node);
}


struct Node* insert( Node* node, int data)
{
    if (node == NULL) return newNode(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);

    return node;
}

int isSameStructure(Node* a, Node* b)
{
	if (a == NULL && b == NULL)
		return 1;
	if (a != NULL && b != NULL)
	{
		return
			(
				isSameStructure(a->left, b->left) &
				isSameStructure(a->right, b->right)
				);
	}
	return 0;
}

int max_depth(Node* node)  {
	int left_depth = 0;

	 if (node->left) {
		 left_depth = max_depth(node->left);
	 }

	 int right_depth = 0;
	 if (node->right) {
		 int right_depth = max_depth(node->right);

	 }
	return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

struct cell_display {
	string   valstr;
	bool     present;
	cell_display() : present(false) {}
	cell_display(std::string valstr) : valstr(valstr), present(true) {}
};

using display_rows = vector< vector< cell_display > >;

display_rows get_row_display(Node* root) {
	vector<Node*> traversal_stack;
	vector< std::vector<Node*> > rows;
	if (!root) return display_rows();

	Node* p = root;
	const int maxDepth = max_depth(root);
	rows.resize(maxDepth);
	int depth = 0;
	for (;;) {
		if (depth == maxDepth - 1) {
			rows[depth].push_back(p);
			if (depth == 0) break;
			--depth;
			continue;
		}

		if (traversal_stack.size() == depth) {
			rows[depth].push_back(p);
			traversal_stack.push_back(p);
			if (p) p = p->left;
			++depth;
			continue;
		}

		if (rows[depth + 1].size() % 2) {
			p = traversal_stack.back();
			if (p) p = p->right;
			++depth;
			continue;
		}

		if (depth == 0) break;

		traversal_stack.pop_back();
		p = traversal_stack.back();
		--depth;
	}

	display_rows rows_disp;
	std::stringstream ss;
	for (const auto& row : rows) {
		rows_disp.emplace_back();
		for (Node* pn : row) {
			if (pn) {
				ss << pn->data;
				rows_disp.back().push_back(cell_display(ss.str()));
				ss = std::stringstream();
			}
			else {
				rows_disp.back().push_back(cell_display());
			}
		}
	}
	return rows_disp;
}

vector<string> row_formatter(const display_rows& rows_disp) {
	using s_t = string::size_type;

	s_t cell_width = 0;
	for (const auto& row_disp : rows_disp) {
		for (const auto& cd : row_disp) {
			if (cd.present && cd.valstr.length() > cell_width) {
				cell_width = cd.valstr.length();
			}
		}
	}

	if (cell_width % 2 == 0) ++cell_width;

	if (cell_width < 3) cell_width = 3;


	vector<string> formatted_rows;

	s_t row_count = rows_disp.size();

	s_t row_elem_count = static_cast<s_t>(1) << (row_count - 1);

	s_t left_pad = 0;

	for (s_t r = 0; r < row_count; ++r) {
		const auto& cd_row = rows_disp[row_count - r - 1]; 
		s_t space = (s_t(1) << r) * (cell_width + 1) / 2 - 1;
		string row;
		for (s_t c = 0; c < row_elem_count; ++c) {
			row += string(c ? left_pad * 2 + 1 : left_pad, ' ');
			if (cd_row[c].present) {
				const string& valstr = cd_row[c].valstr;
				s_t long_padding = cell_width - valstr.length();
				s_t short_padding = long_padding / 2;
				long_padding -= short_padding;
				row += string(c % 2 ? short_padding : long_padding, ' ');
				row += valstr;
				row += string(c % 2 ? long_padding : short_padding, ' ');
			}
			else {
				row += string(cell_width, ' ');
			}
		}
		formatted_rows.push_back(row);

		if (row_elem_count == 1) break;

		s_t left_space = space + 1;
		s_t right_space = space - 1;
		for (s_t sr = 0; sr < space; ++sr) {
			string row;
			for (s_t c = 0; c < row_elem_count; ++c) {
				if (c % 2 == 0) {
					row += string(c ? left_space * 2 + 1 : left_space, ' ');
					row += cd_row[c].present ? '/' : ' ';
					row += string(right_space + 1, ' ');
				}
				else {
					row += string(right_space, ' ');
					row += cd_row[c].present ? '\\' : ' ';
				}
			}
			formatted_rows.push_back(row);
			++left_space;
			--right_space;
		}
		left_pad += space + 1;
		row_elem_count /= 2;
	}

	std::reverse(formatted_rows.begin(), formatted_rows.end());

	return formatted_rows;
}

void trim_rows_left(vector<string>& rows) {
	if (!rows.size()) return;
	auto min_space = rows.front().length();
	for (const auto& row : rows) {
		auto i = row.find_first_not_of(' ');
		if (i == string::npos) i = row.length();
		if (i == 0) return;
		if (i < min_space) min_space = i;
	}
	for (auto& row : rows) {
		row.erase(0, min_space);
	}
}

void Dump(Node *node)  {
	const int d = max_depth(node);

	if (d == 0) {
		cout << " <empty tree>\n";
		return;
	}

	const auto rows_disp = get_row_display(node);
	auto formatted_rows = row_formatter(rows_disp);
	trim_rows_left(formatted_rows);
	for (const auto& row : formatted_rows) {
		std::cout << ' ' << row << '\n';
	}
}

int main()
{
	Node* root1 = newNode(10);
	Node* root2 = newNode(100);
	root1->left = newNode(7);
	root1->right = newNode(15);
	root1->left->left = newNode(4);
	root1->left->right = newNode(9);
	root1->right->right = newNode(20);

	root2->left = newNode(70);
	root2->right = newNode(150);
	root2->left->left = newNode(40);
	root2->left->right = newNode(90);
	root2->right->right = newNode(201);

	cout << "Node 1: \n";
	Dump(root1);
	cout << "Node 2: \n";
	Dump(root2);


	if (isSameStructure(root1, root2))
		printf("Both trees have same structure");
	else
		printf("Trees do not have same structure");
	return 0;
}