export class Gitlitehub {
    constructor(id) {
        this.id = id;
        this.$gitlitehub = $('#' + id);

        this.fileExplorer = new FileExplorer(this);
    }
};
