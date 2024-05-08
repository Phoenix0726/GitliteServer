class UserInfo {
    constructor(root) {
        this.root = root;

        this.start();
    }

    start() {
        // this.register();
        // this.login();
        this.getInfo();
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
                    console.log(resp);
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