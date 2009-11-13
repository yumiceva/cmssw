#!/usr/bin/env python

## Utility used to overlay muon HLT plots from several releases

## For more information about available options, use the -h option:
##     ./compareReleases.py -h

## Import python libraries
import sys
import optparse
import os
import re


if '-h' not in sys.argv:

    ## Import ROOT in batch mode
    sys.argv.append('-b')
    import ROOT
    ROOT.gROOT.Macro('rootlogon.C')
    sys.argv.remove('-b') 

    ## Define constants
    file_type = ".pdf"
    path_muon = "/DQMData/HLT/Muon"
    path_dist = "%s/Distributions" % path_muon
    crossChannel = re.compile("HLT_[^_]*_[^_]*$")
    colors = [ROOT.kBlue, ROOT.kRed, ROOT.kGreen+2, ROOT.kMagenta+2,
              ROOT.kYellow+2, ROOT.kCyan+2, ROOT.kBlue+3, ROOT.kRed+3]

    ## Set up style and canvas
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetOptFit(0)
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gErrorIgnoreLevel = ROOT.kWarning
    c1 = ROOT.TCanvas("c1","c1",800,600)
    c1.GetFrame().SetBorderSize(6)
    c1.GetFrame().SetBorderMode(-1)



def main():
    ## Parse command line options
    usage="usage: %prog [options] file1.root file2.root file3.root ..."
    parser = optparse.OptionParser(usage=usage)
    parser.add_option('-p', '--path', default="HLT_Mu9",
                      help="specify which HLT path to focus on")
    parser.add_option('-g', '--gen', action="store_true", default=False,
                      help="skip plots which match to reconstructed muons")
    parser.add_option('-r', '--rec', action="store_true", default=False,
                      help="skip plots which match to generated muons")
    options, arguments = parser.parse_args()
    path = options.path
    source_types = ["gen", "rec"]
    if options.gen: source_types.remove("rec")
    if options.rec: source_types.remove("gen")

    ## Make plots
    if len(arguments) == 0:
        print "Please provide at least one file as an argument"
        return
    print "Generating plots for %s path..." % path
    files, path_names = get_files_and_path_names(arguments)
    efficiencies = get_global_efficiencies(files, path)
    filter_names, me_values = get_filter_names_and_me_values(files, path)
    make_efficiency_summary_plot(files, me_values)
    for source_type in ["gen", "rec"]:
        if source_type in source_types:
            for plot_var in ["TurnOn1", "EffEta", "EffPhi"]:
                for filter in filter_names:
                    plot_name = "%s%s_%s" % (source_type, plot_var, filter)
                    make_efficiency_plot(files, plot_name, path,
                                         efficiencies, me_values)
    for source_type in ["gen"]:
        if source_type in source_types:
            for path in path_names:
                filter_names, me_values = get_filters_and_MEs(files, path)
                for filter in filter_names:
                    plot_name = "%s%s_%s" % ("gen", "TurnOn1", filter)
                    make_efficiency_plot(files, plot_name, path,
                                         efficiencies, me_values)
    if file_type == ".pdf": mergeOutput(files)



def counter_generator():
    k = 0
    while True:
        k += 1
        yield k
next_counter = counter_generator().next



def get_files_and_path_names(arguments):
    files = []
    for filename in arguments:
        files.append(ROOT.TFile(filename, "read"))
    path_names = []
    keys = files[0].GetDirectory(path_dist).GetListOfKeys()
    obj = keys.First()
    while obj:
        path_names.append(obj.GetName())
        obj = keys.After(obj)
    return files, path_names



def get_global_efficiencies(files, path):
    efficiencies = []
    for file_index, this_file in enumerate(files):
        eff_hist = ROOT.TH1F(this_file.Get("%s/%s/globalEfficiencies" %
                                           (path_dist, path)))
        eff_hist.LabelsDeflate()
        efficiencies.append({})
        for i in range(eff_hist.GetNbinsX()):
            label = eff_hist.GetXaxis().GetBinLabel(i + 1)
            eff = 100. * eff_hist.GetBinContent(i + 1)
            err = 100. * eff_hist.GetBinError(i + 1)
            efficiencies[file_index][label] = [eff, err]
    return efficiencies



def get_filters_and_MEs(files, path):
    filter_names = []
    me_values = {}
    regexp = ROOT.TPRegexp("^<(.*)>(i|f|s|qr)=(.*)</\\1>$")
    keys = files[0].GetDirectory("%s/%s" % (path_dist, path)).GetListOfKeys()
    obj = keys.First()
    while obj:
        name = obj.GetName()
        filter_name = name[name.find("_") + 1:len(name)]
        if "genPassPhi" in name:
            if "L1" in name or "L2" in name or "L3" in name:
                filter_names.append(filter_name)
        if regexp.Match(name):
            array = ROOT.TObjArray(regexp.MatchS(name))
            me_values[array.At(1).GetName()] = float(array.At(3).GetName())
        obj = keys.After(obj)
    return filter_names, me_values



def plot(hists, hist_name, hist_title,
         efficiencies=None, me_values=None, path=None):
    plot_num = next_counter()
    first = hists.First()
    last = hists.Last()
    hist = last
    x_title = first.GetXaxis().GetTitle()
    if   "Gen" in x_title: source_type = "gen"
    elif "Rec" in x_title: source_type = "reco"
    expression = ("0.5 * [2] * (" +
                  "TMath::Erf((x / [0] + 1.) / (TMath::Sqrt(2.) * [1])) + " +
                  "TMath::Erf((x / [0] - 1.) / (TMath::Sqrt(2.) * [1])))")
    function_turn_on = ROOT.TF1("turnOn", expression, 10, 40)
    while hist:
        hist_index = int(hists.IndexOf(hist))
        hist.Draw()
        ROOT.gPad.SetLogx(False)
        ROOT.gPad.SetTickx(1)
        title = hist.GetTitle()
        hist.SetLineWidth(2)
        hist.SetLineColor(colors[hist_index])
        hist.Scale(100.)
        hist.GetYaxis().SetRangeUser(0., 100.)
        if "Summary" in hist_name:
            hist.GetXaxis().LabelsOption("u")
            c1.SetTickx(0)
        if "Eff" in hist_name or "TurnOn" in hist_name:
            yTitle = hist.GetYaxis().GetTitle()
            slashIndex = yTitle.find("/")
            yTitle = "#frac{%s}{%s} (%%)" % (yTitle[0:slashIndex - 1],
                                            yTitle[slashIndex + 2:100])
            hist.GetYaxis().SetTitle(yTitle)
            hist.GetYaxis().SetTitleOffset(1.5)
            hist.GetYaxis().SetTitleSize(0.04)
        if "Eff" in hist_name:
            eff, err = efficiencies[hist_index][hist_name]
            hist.SetTitle("%s (%.1f#pm%.1f%%)" % (title, eff, err))
        if "TurnOn" in hist_name:
            ROOT.gPad.SetLogx(True)
            hist.GetXaxis().SetRangeUser(2., 300.)
            function_turn_on.SetParameters(1,20,100)
            function_turn_on.SetLineColor(hist.GetLineColor())
            hist.Fit("turnOn","q")
            hist.Draw()
            eff = function_turn_on.GetParameter(2)
            if eff < 100 and eff > 0:
                hist.SetTitle("%s (%.1f%%)" % (title, eff))
        hist = hists.Before(hist)
    last.Draw()
    hist = hists.Before(last)
    while hist:
        hist.Draw("same")
        hist = hists.Before(hist)
    lower_bound_y = 0.15
    upper_bound_y = lower_bound_y + (0.055 * hists.Capacity())
    if   "Summary" in hist_name:
        cuts = None
    elif "TurnOn"  in hist_name:
        cuts = "|#eta| < %.1f" % me_values["CutMaxEta"]
    else:
        cuts = "p_{T} > %.0f, |#eta| < %.1f" % (me_values["CutMinPt"],
                                                me_values["CutMaxEta"])
    legend = ROOT.gPad.BuildLegend(0.22, lower_bound_y, 0.90, upper_bound_y)
    legend.SetFillColor(0)
    legend.SetFillStyle(1001)
    if "Summary" not in hist_name:
        latex = ROOT.TLatex()
        latex.SetNDC()
        latex.SetTextAlign(31)
        latex.DrawLatex(0.93, upper_bound_y + 0.015,
                        "Cuts on #mu_{%s}: %s" % (source_type, cuts))
    hist_title = "%.3i: %s" % (plot_num, hist_title)
    if path: hist_title += " step in %s" % path
    last.SetTitle(hist_title)
    ROOT.gPad.Update()
    c1.SaveAs("%.3i%s" % (plot_num, file_type))



def make_efficiency_summary_plot(files, me_values):
    class Bin:
        def __init__(self, label, content, error):
            self.label = label
            self.content = content
            self.error = error
    hists = ROOT.TList()
    hist_path = "%s/Summary/Efficiency_Summary" % path_muon
    for this_file in files:
        hist = ROOT.TH1F(this_file.Get(hist_path))
        n_bins = hist.GetNbinsX()
        bins = []
        for i in range(n_bins):
            bins.append(Bin(hist.GetXaxis().GetBinLabel(i + 1),
                            hist.GetBinContent(i + 1),
                            hist.GetBinError(i + 1)))
        n_cross = i = 0
        while i != (len(bins) - n_cross):
            if crossChannel.match(bins[i].label):
                bins.append(bins.pop(i))
                n_cross += 1
            else:
                i += 1
        for i, bin in enumerate(bins):
            hist.GetXaxis().SetBinLabel(i + 1, bin.label)
            hist.SetBinContent(i + 1, bin.content)
            hist.SetBinError(i + 1, bin.error)
        hist_clone = hist.Clone()
        hist_clone.SetTitle(filename(this_file))
        hists.Add(hist_clone)
    if hists.At(0):
        plot(hists, "Summary", "Efficiency Summary")



def make_efficiency_plot(files, plot_name, path, efficiencies, me_values):
    hists = ROOT.TList()
    hist_path = "%s/%s/%s" % (path_dist, path, plot_name)
    hist_title = ROOT.TH1F(files[0].Get(hist_path)).GetTitle()
    for this_file in files:
        hist = ROOT.TH1F(this_file.Get(hist_path))
        hist_clone = hist.Clone()
        hist_clone.SetTitle(filename(this_file))
        hists.Add(hist_clone)
    if hists.At(0):
        plot(hists, plot_name, hist_title, efficiencies, me_values, path)



def mergeOutput(files):
    os.system("gs -q -dBATCH -dNOPAUSE -sDEVICE=pdfwrite -dAutoRotatePages=/All  -sOutputFile=merged.pdf [0-9][0-9][0-9].pdf")
    if len(files) == 1:
        pdfname = "%s.pdf" % filename(files[0])
    elif len(files) == 2:
        pdfname = "%s_vs_%s.pdf" % (filename(files[0]), filename(files[1]))
    else:
        pdfname = "%s_vs_Many.pdf" % filename(files[0])
    os.system("cp merged.pdf %s" % pdfname)
    os.system("rm [0-9]*.pdf")
    print "Wrote %i plots to %s" % (next_counter() - 1, pdfname)



def filename(this_file):
    return this_file.GetName()[0:this_file.GetName().find(".root")]



if __name__ == "__main__":
    sys.exit(main())

