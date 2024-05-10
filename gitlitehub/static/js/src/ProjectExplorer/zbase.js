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
        this.getProjects();
    }

    add_listening_events() {
        let outer = this;

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

        this.$projectList.on('click', ".project-list-item-name", function() {
            let name = $(this).text();
            let url = "http://127.0.0.1:8000/" + outer.root.username + "/" + name;
            $(location).attr('href', url);
        });

        this.$projectList.on('click', ".project-list-item-delete", function() {
            let name = $(this).siblings('.project-list-item-name').text();
            let owner = outer.root.username;
            $.confirm({
                title: 'Delete Project: ' + name,
                content: '' +
                '<form action="" class="formName">' +
                '<div class="form-group">' +
                '<label>Please enter the project name to confirm deleting the project </label>' +
                '<input type="text" placeholder="Project name" class="name form-control" required />' +
                '</div>' +
                '</form>',
                buttons: {
                    formSubmit: {          
                        text: 'Confirm',           
                        btnClass: 'btn-red',            
                        action: function () {           
                            let text = this.$content.find('.name').val();           
                            if(text !== name){    
                                $.alert('Error project name');     
                                return false;         
                            }
                            outer.deleteProject(name, owner);
                        }
                    },          
                    cancel: function () {  
                        //close
                    },
                },
            });
        })

        this.$newProject.find('.new-project-submit').click(() => {
            let owner = this.root.username;
            let name = this.$newProject.find('.new-project-name input').val();
            let description = this.$newProject.find('.new-project-description input').val();
            this.createProject(name, owner, description);
        });
    }

    hide() {
        this.$projectList.hide();
        this.$newProject.hide();
    }

    getProjects() {
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
        this.$projectList.find('.project-list-item').remove();
        for (let i in this.projects) {
            let project = this.projects[i];
            let $item = $(`
                <div class="project-list-item">
                    <div class="project-list-item-name">${project.name}</div>
                    <div class="project-list-item-description">${project.description}</div>
                    <div class="project-list-item-stars">⭐ ${project.stars}</div>
                    <div class="project-list-item-delete">🗑</div>
                </div>
            `);
            this.$projectList.append($item);
        }
    }

    createProject(name, owner, description) {
        $.ajax({
            url: "create_project/",
            type: "GET",
            async: false,
            data: {
                'name': name,
                'owner': owner,
                'description': description,
            },
            success: resp => {
                if (resp.result === "success") {
                    url = "http://127.0.0.1/" + owner + "/" + name;
                    $(location).attr('href', url);
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    deleteProject(name, owner) {
        $.ajax({
            url: "delete_project/",
            type: "GET",
            async: false,
            data: {
                'name': name,
                'owner': owner,
            },
            success: resp => {
                if (resp.result === "success") {
                    this.getProjects();
                    this.hide();
                    this.$projectList.show();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }
}