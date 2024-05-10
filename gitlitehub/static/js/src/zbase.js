export class Gitlitehub {
    constructor(id, username, project) {
        this.id = id;
        this.$gitlitehub = $('#' + id);
        
        this.username = username;
        this.project = project;
        console.log(username, project);

        this.fileExplorer = new FileExplorer(this);
        this.projectExplorer = new ProjectExplorer(this);
        this.menu = new Menu(this);
    }
};
