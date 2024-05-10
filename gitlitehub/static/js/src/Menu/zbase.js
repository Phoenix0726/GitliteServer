class Menu {
    constructor(root) {
        this.root = root;
        this.$menu = $(`
            <div class="menu">
                <div class="menu-bar">
                    <div class="menu-item menu-code item-active">Code</div>
                    <div class="menu-item menu-projects">Projects</div>
                    <div class="menu-item menu-settings">Settings</div>
                    <div class="menu-item menu-login">Login</div>
                    <div style="clear: both;"></div>
                </div>
            </div>
        `);
        this.$menuCode = this.$menu.find(".menu-code");
        this.$menuProjects = this.$menu.find(".menu-projects");
        this.$menuSettings = this.$menu.find(".menu-settings");

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
        let menuList = this.$menu.find('.menu-item');
        for (let i = 0; i < menuList.length; i++) {
            $(menuList[i]).click(function() {
                $(this).addClass('item-active').siblings().removeClass('item-active');
            });
        }

        this.$menuCode.click(() => {
            this.hide();
            this.$fileExplorer.show();
        });

        this.$menuProjects.click(() => {
            this.hide();
            this.$projectExplorer.show();
        });
    }

    hide() {
        this.$fileExplorer.hide();
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
                    // console.log(resp);
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