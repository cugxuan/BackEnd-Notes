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
    includeLevel: 2,
    // 换行
    extendMarkdown: md => {
      md.set({breaks: true, linkify: true});
      md.use(require("markdown-it"));
    }
  },
  themeConfig: {
    //显示所有页面的标题链接
    // displayAllHeaders: true,
    sidebarDepth: 2,
    nav: [
      {
        text: "目录",
        items: [
          {
            text: "首页",
            link: "/"
          },
          {
            text: "数据结构",
            link: "/Data-Structure/"
          }
        ]
      },
      {
        text: "Github",
        link: "https://github.com/cugxuan/BackEnd-Notes"
      }
    ],
    sidebar: [
      {
        title: '首页',   // 分组标题
        // path: '/',      // 如果你想让他能点击，就取消注释
        collapsable: false, // 是否折叠起来
        sidebarDepth: 2,    // 嵌套层级 默认值是 1
        children: [
          '/'
        ]
      },
      {
        title: '数据结构',   // 分组标题
        path: '/data-Structure/',
        collapsable: true, // 是否折叠起来
        sidebarDepth: 2,    // 嵌套层级 默认值是 1
        children: [
          "data-structure/二进制",
          "data-structure/二分法",
          "data-structure/牛顿迭代法"
        ]
      },
      {
        title: '剑指 offer',
        path: '/jz-offer',
        collapsable: true, // 是否折叠起来
        sidebarDepth: 2,    // 嵌套层级 默认值是 1
        children: [
          "jz-offer/1-二维数组中的查找",
          "jz-offer/2-替换空格",
          "jz-offer/3-从尾到头打印链表",
          "jz-offer/28-数组中出现次数超过一半的数字",
        ]
      }
    ]
  }
};
