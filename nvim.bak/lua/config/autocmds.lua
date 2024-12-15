vim.api.nvim_create_autocmd("VimEnter", {
  callback = function()
    local path = vim.fn.expand("%:p")
    if vim.fn.isdirectory(path) == 1 then
      require("telescope").extensions.file_browser.file_browser({ cwd = path })
      return true
    end
  end,
})

