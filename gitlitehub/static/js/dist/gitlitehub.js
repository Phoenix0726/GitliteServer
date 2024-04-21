class FileExplorer {
    constructor(root) {
        this.root = root;
        this.$fileExplorer = $(`
            <div class="file-region">
            </div>
        `);
        this.root.$gitlitehub.append(this.$fileExplorer);

        this.path = "./";

        this.start();
    }

    start() {
        this.get_filelist();

        this.add_listening_events();
    }

    add_listening_events() {
        let outer = this;
        this.$fileExplorer.on('click', ".file-item", function() {
            let filename = $(this).text();
            let filetype = outer.filelist[filename];
            if (filetype === "dir") {
                outer.$fileExplorer.find(".file-item").remove();
                outer.path = outer.path + '/' + filename;
                outer.get_filelist();
            } else if (filetype == "file") {
                outer.$fileExplorer.find(".file-item").remove();
                outer.path = outer.path + '/' + filename;
                outer.get_file_content();
            }
        });
    }

    get_filelist() {
        $.ajax({
            url: "get_filelist/",
            type: "GET",
            data: {
                path: this.path,
            },
            success: resp => {
                if (resp.result === "success") {
                    this.filelist = JSON.parse(JSON.stringify(resp.filelist));
                    this.show_filelist();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    get_file_content() {
        $.ajax({
            url: "get_file_content/",
            type: "GET",
            data: {
                path: this.path,
            },
            success: resp => {
                if (resp.result === "success") {
                    this.content = JSON.parse(JSON.stringify(resp.content));
                    this.show_file_content();
                } else {
                    console.log(resp.result);
                }
            },
            error: e => {
                console.log(e);
            },
        });
    }

    show_filelist() {
        for (let file in this.filelist) {
            let $item = $(`
                <div class="file-item">${file}</div>
            `);
            this.$fileExplorer.append($item);
        }
    }

    show_file_content() {
        let $content = $(`
            <div class="file-content">${this.content}</div>
        `);
        this.$fileExplorer.append($content);
    }
}
export class Gitlitehub {
    constructor(id) {
        this.id = id;
        this.$gitlitehub = $('#' + id);

        this.fileExplorer = new FileExplorer(this);
    }
};
