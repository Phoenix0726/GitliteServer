export class Gitlitehub {
    constructor(id, username, project) {
        this.id = id;
        this.$gitlitehub = $('#' + id);
        
        this.username = username;
        this.project = project;

        this.fileExplorer = new FileExplorer(this);
        this.projectExplorer = new ProjectExplorer(this);
        this.userPanel = new UserPanel(this);
        this.menu = new Menu(this);
    }
};
