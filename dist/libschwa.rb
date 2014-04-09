require "formula"

class Libschwa < Formula
  homepage "https://github.com/schwa-lab/libschwa"
  url "https://github.com/schwa-lab/libschwa/releases/download/0.2.2/libschwa-0.2.2.tar.gz"
  sha1 "41d3e1a2dd3686da6ee0acb302e88eb84ca937bb"

  depends_on "zeromq" => :optional

  def install
    system "./configure", "--prefix=#{prefix}"
    system "make"
    system "make", "check"
    system "make", "install"
  end

  test do
    system "#{bin}/schwa-tokenizer", "--version"
  end
end
