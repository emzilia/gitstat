#include <iostream>
#include <vector>
#include <filesystem>
#include <git2.h>

std::vector<std::string> repo_dirs;
std::vector<std::string>::iterator ri;

std::vector<std::string> get_repos() {
	std::string home_path = getenv("HOME");
	std::string repo_path = home_path + "/repos/";

	for (const auto & entry : std::filesystem::directory_iterator(repo_path)) {
		if (entry.is_directory()) {
			if (git_repository_open_ext(
						NULL, entry.path().c_str(), GIT_REPOSITORY_OPEN_NO_SEARCH, NULL
						) == 0) {
				repo_dirs.push_back(entry.path());
			}
		}
	}

	return repo_dirs;
}

int main() {
	git_libgit2_init();

	repo_dirs = get_repos();

	for (ri = repo_dirs.begin(); ri != repo_dirs.end(); ++ri) {
		std::cout << *ri << std::endl;
	}
	
	git_libgit2_shutdown();
}
