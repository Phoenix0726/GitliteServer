class UserPanel {
    constructor(root) {
        this.root = root;
        this.$userPanel = $(`
            <div class="user-panel">
                <div class="user-panel-header">
                    <div class="user-panel-header-photo">
                        <img src="https://logos-download.com/wp-content/uploads/2016/09/GitHub_logo.png"/>
                    </div>
                    <div class="user-panel-header-name">
                        <span class="user-panel-header-username">username</span>
                        <span class="user-panel-header-tickname">tickname</span>
                    </div>
                </div>
                <div class="user-panel-menu">
                    <div class="user-panel-menu-item user-panel-menu-profile">My profile</div>
                    <div class="user-panel-menu-item user-panel-menu-projects">My projects</div>
                    <div class="user-panel-menu-item user-panel-menu-organizations">My organizations</div>
                    <div class="user-panel-menu-item user-panel-menu-stars">My stars</div>
                </div>
            </div>
        `);
        this.$userPanel.hide();
        this.$myProjects = this.$userPanel.find(".user-panel-menu-projects");

        this.root.$gitlitehub.append(this.$userPanel);

        this.start();
    }

    start() {
        this.getInfo();

        this.add_listening_events();
    }

    add_listening_events() {
        this.$userPanel.mouseleave(() => {
            this.$userPanel.hide();
        });

        this.$myProjects.click(() => {
            this.root.menu.hide();
            this.root.menu.projectExplorerShow();
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
                    this.username = JSON.parse(JSON.stringify(resp.username));
                    let $userPhoto = this.$userPanel.find('.user-panel-header-photo img');
                    $userPhoto.attr('src', this.photo);
                    let $username = this.$userPanel.find('.user-panel-header-username');
                    $username.text(this.username);
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