#include <iostream>
#include <vector>
#include <filesystem>
#include <git2.h>

std::vector<std::string> repo_dirs;
//std::vector<git_repository> repos_dirs;
std::vector<std::string>::iterator ri;

std::vector<std::string> get_repos() {
	std::string home_path = getenv("HOME");
	std::string repo_path = home_path + "/repos/";

	for (const auto &entry : std::filesystem::directory_iterator(repo_path)) {
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

void get_statuses(std::vector<std::string> repos) {
	/*
	git_cred *cred = NULL;
	git_credential_acquire_cb(&cred, remote

	git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;

	fetch_opts.callbacks.credentials =
	*/

	for (ri = repo_dirs.begin(); ri != repo_dirs.end(); ++ri) {
		std::string repo_string = *ri;
		git_repository *repo = NULL;
		git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;
		git_remote *remote = NULL;

		if (git_repository_open(&repo, repo_string.c_str()) == 0) {
			git_remote_lookup(&remote, repo, "origin");
			if (git_remote_fetch(remote, NULL, &fetch_opts, NULL) == 0) {
				std::cout << "Remote changes fetched for: " << *ri << std::endl;
			} else {
				std::cout << "Error fetching remote changes for: " << *ri << std::endl;
				perror("git_remote_fetch");
				const git_error *err = git_error_last();
				fprintf(stderr, "%s\n", err->message);
				exit(EXIT_FAILURE);
			}
		} else {
			perror("git_repository_open");
			exit(EXIT_FAILURE);
		}
	}
}

int main() {
	git_libgit2_init();

	repo_dirs = get_repos();

	get_statuses(repo_dirs);

	git_libgit2_shutdown();
}
