vim.api.nvim_create_user_command("Ex", function()
  local path = vim.fn.expand("%:p:h") -- Current file's directory
  if path == "" or vim.fn.isdirectory(path) == 0 then
    path = vim.fn.getcwd() -- Fall back to the working directory
  end
  require("telescope").extensions.file_browser.file_browser({ cwd = path })
end, { desc = "Open Telescope File Browser dynamically" })

