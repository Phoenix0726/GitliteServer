class Menu {
    constructor(root) {
        this.root = root;
        this.$menu = $(`
            <div class="menu">
                <div class="top-navigation">
                    <div class="nav-project-path">
                        <img src="https://logos-download.com/wp-content/uploads/2016/09/GitHub_logo.png"/>
                        <span class="nav-project-path-username">${this.root.username}</span> / 
                        <span class="nav-project-path-projectname">${this.root.project}</span>
                    </div>
                    <div class="nav-search-bar">
                        <input type="text" value=" 🔍 "/>
                    </div>
                    <div class="nav-user-photo">
                        <img src="https://logos-download.com/wp-content/uploads/2016/09/GitHub_logo.png"/>
                    </div>
                </div>
                <div class="menu-bar menu-file-explorer">
                    <div class="menu-item menu-code item-active">Code</div>
                    <div class="menu-item menu-issue">Issues</div>
                    <div style="clear: both;"></div>
                </div>
                <div class="menu-bar menu-project-explorer">
                    <div class="menu-item menu-projects item-active">Projects</div>
                    <div class="menu-item menu-stars">Stars</div>
                    <div style="clear: both;"></div>
                </div>
            </div>
        `);
        this.$menuFileExplorer = this.$menu.find(".menu-file-explorer");

        this.$menuProjectExplorer = this.$menu.find(".menu-project-explorer");
        this.$menuProjectExplorer.hide();

        this.$menuCode = this.$menu.find(".menu-code");
        this.$menuProjects = this.$menu.find(".menu-projects");

        this.$fileExplorer = this.root.fileExplorer.$fileExplorer;
        this.$projectExplorer = this.root.projectExplorer.$projectExplorer;

        this.root.$gitlitehub.prepend(this.$menu);

        this.start();
    }

    start() {
        this.add_listening_events();
        // this.register();
        // this.login();
        this.getInfo();
    }
    
    add_listening_events() {
        this.$menu.find('.nav-user-photo').click(() => {
            this.root.userPanel.$userPanel.show();
        });

        let menuList = this.$menu.find('.menu-item');
        for (let i = 0; i < menuList.length; i++) {
            $(menuList[i]).click(function() {
                $(this).addClass('item-active').siblings().removeClass('item-active');
            });
        }

        this.$menuCode.click(() => {
            this.hide();
            this.fileExplorerShow();
        });

        this.$menuProjects.click(() => {
            this.hide();
            this.projectExplorerShow()
        });
    }

    hide() {
        this.fileExplorerHide();
        this.projectExplorerHide();
    }

    fileExplorerShow() {
        this.$menuFileExplorer.show();
        this.$fileExplorer.show();
    }

    fileExplorerHide() {
        this.$menuFileExplorer.hide();
        this.$fileExplorer.hide();
    }

    projectExplorerShow() {
        this.$menuProjectExplorer.show();
        this.$projectExplorer.show();
    }

    projectExplorerHide() {
        this.$menuProjectExplorer.hide();
        this.$projectExplorer.hide();
    }

    register() {
        $.ajax({
            url: "register/",
            type: "GET",
            async: false,
            data: {
                username: 'test1',
                password: '1234',
                password_confirm: '1234',
            },
            success: resp => {
                if (resp.result === "success") {
                    console.log('register success');
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    login() {
        $.ajax({
            url: "login/",
            type: "GET",
            async: false,
            data: {
                username: 'test1',
                password: '1234',
            },
            success: resp => {
                if (resp.result === "success") {
                    console.log('login success');
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    getInfo() {
        $.ajax({
            url: "get_info/",
            type: "GET",
            async: false,
            data: {
            },
            success: resp => {
                if (resp.result === "success") {
                    this.photo = JSON.parse(JSON.stringify(resp.photo));
                    let $navUserPhoto = this.$menu.find('.nav-user-photo img');
                    $navUserPhoto.attr('src', this.photo);
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