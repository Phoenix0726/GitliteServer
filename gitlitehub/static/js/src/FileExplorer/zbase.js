class FileExplorer {
    constructor(root) {
        this.root = root;
        this.$fileExplorer = $(`
            <div class="file-explorer">
                <div class="fe-select-menu">
                    <div class="fe-select-menu-item fe-select-menu-files item-active">Files</div>
                    <div class="fe-select-menu-item fe-select-menu-branches">Branches</div>
                    <div class="fe-select-menu-item fe-select-menu-commits">Commits</div>
                    <div style="clear: both;"></div>
                </div>
                <div class="branch-list"></div>
                <div class="commit-list"></div>
                <div class="file-region"></div>
            </div>
        `);
        this.$files = this.$fileExplorer.find('.fe-select-menu-files');
        this.$branches = this.$fileExplorer.find('.fe-select-menu-branches');
        this.$commits = this.$fileExplorer.find('.fe-select-menu-commits');
        this.$fileRegion = this.$fileExplorer.find('.file-region');
        this.$branchList = this.$fileExplorer.find('.branch-list');
        this.$branchList.hide();
        this.$commitList = this.$fileExplorer.find('.commit-list');
        this.$commitList.hide();

        this.root.$gitlitehub.append(this.$fileExplorer);

        this.project = this.root.project;
        this.path = this.root.username + '/' + this.project;

        this.start();
    }

    start() {
        this.getBranches();
        this.getCommits();
        this.checkoutCommit(this.commits[0]);
        this.getFilelist();

        this.addListeningEvents();        
    }

    addListeningEvents() {
        let outer = this;

        let menuList = this.$fileExplorer.find('.fe-select-menu-item');
        for (let i = 0; i < menuList.length; i++) {
            $(menuList[i]).click(function() {
                $(this).addClass('item-active').siblings().removeClass('item-active');
            });
        }

        this.$files.click(() => {
            this.hide();
            this.$fileRegion.show();
        });

        this.$branches.click(() => {
            this.hide();
            this.$branchList.show();
        });

        this.$commits.click(() => {
            this.hide();
            this.$commitList.show();
        });

        this.$branchList.on('click', '.branch-list-item', function() {
            let branch = $(this).text();
            console.log(branch);
            if (branch[0] !== '*') {
                outer.checkoutBranch(branch);
            }
            outer.hide();
            outer.$fileRegion.show();
        });

        this.$commitList.on('click', '.commit-list-item', function() {
            let commit = $(this).text().slice(7, 47);
            console.log(commit);
            outer.checkoutCommit(commit);
            outer.hide();
            outer.getFilelist();
            outer.$fileRegion.show();
        });

        this.$fileRegion.on('click', ".file-item-name", function() {
            let filename = $(this).text();
            let filetype = outer.filelist[filename];
            if (filetype === "dir") {
                outer.updateFilepath(filename);
                outer.getFilelist();
            } else if (filetype == "file") {
                outer.updateFilepath(filename);
                outer.getFileContent();
            }
        });
    }

    getBranches() {
        $.ajax({
            url: "get_branches/",
            type: "GET",
            async: false,
            data: {
                project: this.project,
            },
            success: resp => {
                if (resp.result === "success") {
                    this.branches = JSON.parse(JSON.stringify(resp.branches));
                    this.curBranch = JSON.parse(JSON.stringify(resp.cur_branch));
                    this.showBranches();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    getCommits() {
        $.ajax({
            url: "get_commits/",
            type: "GET",
            async: false,
            data: {
                project: this.project,
            },
            success: resp => {
                if (resp.result === "success") {
                    this.commits = JSON.parse(JSON.stringify(resp.commits));
                    this.commitsMsg = JSON.parse(JSON.stringify(resp.commits_msg));
                    this.showCommits();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    checkoutBranch(branch) {
        $.ajax({
            url: "checkout_branch/",
            type: "GET",
            async: false,
            data: {
                project: this.project,
                branch: branch,
            },
            success: resp => {
                if (resp.result === "success") {
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    checkoutCommit(commit) {
        $.ajax({
            url: "checkout_commit/",
            type: "GET",
            async: false,
            data: {
                project: this.project,
                commit: commit,
            },
            success: resp => {
                if (resp.result === "success") {
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    getFilelist() {
        $.ajax({
            url: "get_filelist/",
            type: "GET",
            async: false,
            data: {
                project: this.project,
                path: this.path,
            },
            success: resp => {
                if (resp.result === "success") {
                    this.filelist = JSON.parse(JSON.stringify(resp.filelist));
                    this.showFilelist();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    getFileContent() {
        $.ajax({
            url: "get_file_content/",
            type: "GET",
            async: false,
            data: {
                path: this.path,
            },
            success: resp => {
                if (resp.result === "success") {
                    this.content = JSON.parse(JSON.stringify(resp.content));
                    this.showFileContent();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    updateFilepath(filename) {
        if (filename == '..') {
            while (this.path[this.path.length - 1] != '/') {
                this.path = this.path.slice(0, -1);
            }
            this.path = this.path.slice(0, -1);
        } else {
            this.path = this.path + '/' + filename;
        }
    }

    showBranches() {
        this.$branchList.find('.branch-list-item').remove();
        for (let i = 0; i < this.branches.length; i++) {
            let branch = this.branches[i];
            if (branch == this.curBranch) branch = "*" + branch;
            let $item = $(`
                <div class="branch-list-item">${branch}</div>
            `);
            this.$branchList.append($item);
        }
    }

    showCommits() {
        this.$commitList.find('.commit-list-item').remove();
        for (let i = 0; i < this.commits.length; i++) {
            let commitMsg = this.commitsMsg[this.commits[i]];
            let $item = $(`
                <div class="commit-list-item">${commitMsg}</div>
            `);
            this.$commitList.append($item);
        }
    }

    showFilelist() {
        this.$fileRegion.find('.file-path').remove();
        let $path = $(`<div class="file-path">${this.path}</div>`);
        this.$fileRegion.append($path);

        this.$fileRegion.find(".file-item").remove();
        for (let file in this.filelist) {
            let icon = '📄';
            if (this.filelist[file] === 'dir') icon = '📂';
            let $item = $(`
                <div class="file-item">
                    ${icon} <span class="file-item-name">${file}</span>
                </div>
            `);
            this.$fileRegion.append($item);
        }
    }

    showFileContent() {
        this.$fileRegion.find(".file-item").remove();
        let $content = $(`
            <div class="file-content">${this.content}</div>
        `);
        this.$fileRegion.append($content);
    }

    hide() {
        this.$fileRegion.hide();
        this.$branchList.hide();
        this.$commitList.hide();
    }
}
