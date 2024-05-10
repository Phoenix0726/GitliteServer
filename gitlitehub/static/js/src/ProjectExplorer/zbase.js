class ProjectExplorer {
    constructor(root) {
        this.root = root;
        this.$projectExplorer = $(`
            <div class="project-explorer">
                <div class="pe-select-menu">
                    <div class="pe-select-menu-item pe-select-menu-projects item-active">Projects</div>
                    <div class="pe-select-menu-item pe-select-menu-new">New</div>
                    <div style="clear: both;"></div>
                </div>
                <div class="project-list"></div>
                <div class="new-project">
                    <div class="new-project-item new-project-name">
                        <span>Project Name:</span>
                        <input/>
                    </div>
                    <div class="new-project-item new-project-description">
                        <span>Description:</span>
                        <input/>
                    </div>
                    <button type="submit" class="new-project-submit">
                        Create project
                    </button>
                </div>
            </div>
        `);
        this.$projectExplorer.hide();
        this.$projects = this.$projectExplorer.find('.pe-select-menu-projects');
        this.$new = this.$projectExplorer.find('.pe-select-menu-new');
        this.$projectList = this.$projectExplorer.find('.project-list');
        this.$newProject = this.$projectExplorer.find('.new-project');
        this.$newProject.hide();

        this.root.$gitlitehub.append(this.$projectExplorer);

        this.start();
    }

    start() {
        this.add_listening_events();
        this.getPorjects();
    }

    add_listening_events() {
        let menuList = this.$projectExplorer.find('.pe-select-menu-item');
        for (let i = 0; i < menuList.length; i++) {
            $(menuList[i]).click(function() {
                $(this).addClass('item-active').siblings().removeClass('item-active');
            });
        }

        this.$projects.click(() => {
            this.hide();
            this.$projectList.show();
        });

        this.$new.click(() => {
            this.hide();
            this.$newProject.show();
        });
    }

    hide() {
        this.$projectList.hide();
        this.$newProject.hide();
    }

    getPorjects() {
        $.ajax({
            url: "get_projects/",
            type: "GET",
            async: false,
            data: {
            },
            success: resp => {
                if (resp.result === "success") {
                    this.projects = JSON.parse(JSON.stringify(resp.projects));
                    this.showProjects();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    showProjects() {
        for (let i in this.projects) {
            let $item = $(`
                <div class="project-list-item">${this.projects[i]}</div>
            `);
            this.$projectList.append($item);
        }
    }
}