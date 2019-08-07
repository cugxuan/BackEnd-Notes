module.exports = {
  title: "Backend-Notes",
  description: "校招笔记整理，主要是后端部分",
  base: "/",

  plugins: {
    "@vssue/vuepress-plugin-vssue": {
      // 设置 `platform` 而不是 `api`
      platform: "github",
      
      // 其他的 Vssue 配置
      owner: "cugxuan",
      repo: "BackEnd-Notes",
      clientId: "6cfa3e29f10d6b97eb6b",
      clientSecret: "00d9a190ff30c15709c136c18d82f30d64c316a3"
    }
  },
  markdown: {
    lineNumbers: true,
    // 换行
    extendMarkdown: md => {
      md.set({ breaks: true });
      md.use(require("markdown-it"));
    }
  },
  themeConfig: {
    //显示所有页面的标题链接
    // displayAllHeaders: true,
    sidebarDepth: 3,
    nav: [
      {
        text: "目录",
        items: [
          {
            text: "数据结构",
            link: "/Data-Structure/"
          }
        ]
      }
    ],
    sidebar: {
      sidebarDepth: 3,
      "/Data-Structure/": ["", "二进制", "二分法", "牛顿迭代法"],
      "/": [""]
    }
  }
};
